import pandas as pd
import numpy as np
import plotly.graph_objects as go

df=pd.read_csv("trajectory.csv")
u=np.linspace(0,2*np.pi,50)
v=np.linspace(0,np.pi,50)
x_sphere=np.outer(np.cos(u),np.sin(v))
y_sphere=np.outer(np.sin(u),np.sin(v))
z_sphere=np.outer(np.ones(np.size(u)),np.cos(v))
fig=go.Figure()

fig.add_trace(go.Surface(
	x=x_sphere, y=y_sphere, z=z_sphere,
	opacity=0.1, colorscale='Blues',
	showscale=False, hoverinfo='skip'))

axes=[
	go.Scatter3d(x=[-1, 1], y=[0, 0], z=[0, 0], mode='lines', line=dict(color='gray', dash='dash'), name='X-Axis'),
	go.Scatter3d(x=[0, 0], y=[-1, 1], z=[0, 0], mode='lines', line=dict(color='gray', dash='dash'), name='Y-Axis'),
	go.Scatter3d(x=[0, 0], y=[0, 0], z=[-1, 1], mode='lines', line=dict(color='gray', dash='dash'), name='Z-Axis')	]
for ax in axes: 
	fig.add_trace(ax)

fig.add_trace(go.Scatter3d(
	x=df['x'], y=df['y'], z=df['z'],
	mode='lines+markers',
	marker=dict(size=3, color='darkred'),
	line=dict(color='red', width=5),
	name='Trajectory'))

fig.update_layout(
	title="Larmor Precession: 3D Quantum State Trajectory",
	scene=dict(
		xaxis=dict(range=[-1.1, 1.1], title='<X>'),
		yaxis=dict(range=[-1.1, 1.1], title='<Y>'),
		zaxis=dict(range=[-1.1, 1.1], title='<Z>')
	),
	width=800, height=800)

fig.show()
